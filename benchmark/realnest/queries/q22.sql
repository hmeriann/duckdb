SELECT 
  list_aggregate(tau_pt, 'mean'),
  list_aggregate(tau_eta, 'mean'),
  list_aggregate(jet_pt, 'mean'),
  list_aggregate(jet_eta, 'mean'),
  list_aggregate(muon_pt, 'mean'),
  list_aggregate(muon_eta, 'mean'),
  list_aggregate(el_pt, 'mean'),
  list_aggregate(el_eta, 'mean'),
  list_aggregate(ph_pt, 'mean'),
  list_aggregate(ph_eta, 'mean')  
FROM singleMu;