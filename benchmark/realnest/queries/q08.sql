SELECT 
  list_aggregate(tau_pt, 'entropy'),
  list_aggregate(tau_eta, 'entropy'),
  list_aggregate(jet_pt, 'entropy'),
  list_aggregate(jet_eta, 'entropy'),
  list_aggregate(muon_pt, 'entropy'),
  list_aggregate(muon_eta, 'entropy'),
  list_aggregate(el_pt, 'entropy'),
  list_aggregate(el_eta, 'entropy'),
  list_aggregate(ph_pt, 'entropy'),
  list_aggregate(ph_eta, 'entropy')  
FROM singleMu;