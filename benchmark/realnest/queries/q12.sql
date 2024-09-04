SELECT 
  list_aggregate(tau_pt, 'group_concat'),
  list_aggregate(tau_eta, 'group_concat'),
  list_aggregate(jet_pt, 'group_concat'),
  list_aggregate(jet_eta, 'group_concat'),
  list_aggregate(muon_pt, 'group_concat'),
  list_aggregate(muon_eta, 'group_concat'),
  list_aggregate(el_pt, 'group_concat'),
  list_aggregate(el_eta, 'group_concat'),
  list_aggregate(ph_pt, 'group_concat'),
  list_aggregate(ph_eta, 'group_concat')  
FROM singleMu;